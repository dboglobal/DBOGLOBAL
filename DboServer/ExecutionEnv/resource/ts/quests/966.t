CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 966;
	title = 96602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1053;
				taid = 1;
				type = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98007;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98007;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 6;
			prelnk = "4;3;";

			CDboTSActWorldPlayScript
			{
				sid = 966;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 966;
				start = 0;
				taid = 1;
				uof = 0;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1053;
				taid = 2;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 98007;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 404;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 96609;
				ctype = 1;
				idx = 3613104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 96608;
				area = 96601;
				goal = 96604;
				m0fz = "575.000000";
				m0widx = 1;
				scitem = 98007;
				sort = 96605;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2783.000000";
				m0ttip = 96615;
				rwd = 100;
				taid = 1;
				title = 96602;
				gtype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 96614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 96601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3613104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 96607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 34;
			}
			CDboTSClickNPC
			{
				npcidx = "3613104;";
			}
			CDboTSCheckClrQst
			{
				and = "965;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 96608;
			gtype = 2;
			oklnk = 2;
			area = 96601;
			goal = 96604;
			sort = 96605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 96602;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98007;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1053;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 96607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

