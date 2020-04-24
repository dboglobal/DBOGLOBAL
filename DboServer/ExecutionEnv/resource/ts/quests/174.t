CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 174;
	title = 17402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 447;
			}
			CDboTSClickNPC
			{
				npcidx = "2413101;";
			}
		}
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
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 4;
			idx = 1743;
			lilnk = 255;
			oklnk = 100;
			prelnk = "5;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "103;102;";
			type = 1;
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
				iidx = 447;
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
				iidx0 = 447;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 17409;
				ctype = 1;
				idx = 4181101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 447;
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
				cont = 17408;
				gtype = 1;
				area = 17401;
				goal = 17404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 17405;
				stype = 2;
				taid = 1;
				title = 17402;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 17408;
			gtype = 1;
			oklnk = 2;
			area = 17401;
			goal = 17404;
			sort = 17405;
			prelnk = "0;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 17402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 17407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
			CDboTSCheckClrQst
			{
				and = "173;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 17414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 17401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;4;";
			usetbl = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
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
				iidx = 447;
			}
		}
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
				iidx0 = 447;
				taid = 1;
				type = 1;
			}
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

