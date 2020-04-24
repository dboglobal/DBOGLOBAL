CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1085;
	title = 108502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
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
			stdiag = 108507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655201;";
			}
			CDboTSCheckClrQst
			{
				and = "1082;";
				flink = 1;
				flinknextqid = "1090;";
				not = 0;
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
				conv = 108509;
				ctype = 1;
				idx = 1655201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 6612101;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 6612208;
				taid = 3;
				esctype = 0;
				idx2 = 6612209;
			}
			CDboTSActRegQInfo
			{
				cont = 108508;
				m1fx = "181.119995";
				m2fz = "-201.820007";
				m1fy = "0.000000";
				sort = 108505;
				m2widx = 4;
				grade = 132203;
				m1pat = 1;
				m1widx = 4;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 108501;
				goal = 108504;
				m0fz = "66.279999";
				m0widx = 4;
				m1ttip = 108516;
				m2fy = "0.000000";
				m2pat = 1;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-322.450012";
				m2fx = "338.459991";
				m2ttip = 108517;
				m0fx = "74.029999";
				m0ttip = 108515;
				title = 108502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 108508;
			gtype = 3;
			oklnk = 2;
			area = 108501;
			goal = 108504;
			sort = 108505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 108502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 108514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 108501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1655201;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

