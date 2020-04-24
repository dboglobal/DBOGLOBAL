CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 172;
	title = 17202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 17209;
				ctype = 1;
				idx = 8031101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 446;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 17208;
				gtype = 1;
				area = 17201;
				goal = 17204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 17205;
				stype = 2;
				taid = 1;
				title = 17202;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 102;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 103;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 446;
			}
		}
		CDboTSContGAct
		{
			cid = 103;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 446;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 17207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
			CDboTSCheckClrQst
			{
				and = "171;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 17214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 17201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 446;
			}
			CDboTSClickNPC
			{
				npcidx = "3031203;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "103;102;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 17208;
			gtype = 1;
			oklnk = 2;
			area = 17201;
			goal = 17204;
			sort = 17205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 17202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 446;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 446;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

