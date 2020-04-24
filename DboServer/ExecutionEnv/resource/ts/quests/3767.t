CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3767;
	title = 376702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 376709;
				ctype = 1;
				idx = 3333101;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99165;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 3767;
				cnt0 = 3;
				ectype = -1;
				etype = 0;
				qtidx = 376715;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 376708;
				m1fx = "-3807.560059";
				m2fz = "-6963.819824";
				m1fy = "0.000000";
				sort = 376705;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 376701;
				goal = 376704;
				m0fz = "-6560.830078";
				m0widx = 15;
				m1ttip = 376715;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = 99165;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-6942.459961";
				m2fx = "-4099.299805";
				m2ttip = 376715;
				m0fx = "-3563.229980";
				m0ttip = 376715;
				title = 376702;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
			CDboTSActItem
			{
				iidx0 = 99165;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 376701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3333101;";
			}
			CDboTSCheckClrQst
			{
				and = "3766;";
				flink = 0;
				flinknextqid = "3768;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 376708;
			gtype = 2;
			oklnk = 2;
			area = 376701;
			goal = 376704;
			sort = 376705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 376702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 376707;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99165;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

