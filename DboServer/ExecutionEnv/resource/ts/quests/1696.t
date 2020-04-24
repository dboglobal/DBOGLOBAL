CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1696;
	title = 169602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99058;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
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
				iidx = 99058;
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
				iidx0 = 99058;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 169607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1698;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5311101;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 169614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 169601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5311101;";
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
				conv = 169609;
				ctype = 1;
				idx = 5311101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 169601;
				cnt0 = 2;
				ectype = -1;
				etype = 0;
				qtidx = 169615;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 169608;
				m1fx = "139.910004";
				m1fy = "0.000000";
				sort = 169605;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "45.799999";
				grade = 132203;
				m0fx = "45.480000";
				m0ttip = 169615;
				m1pat = 0;
				m1widx = 9;
				rwd = 100;
				taid = 1;
				title = 169602;
				gtype = 2;
				area = 169601;
				goal = 169604;
				m0fz = "-232.979996";
				m0widx = 9;
				m1ttip = 169615;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 169608;
			gtype = 2;
			oklnk = 2;
			area = 169601;
			goal = 169604;
			sort = 169605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 169602;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
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
				iidx0 = 99058;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 169607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

