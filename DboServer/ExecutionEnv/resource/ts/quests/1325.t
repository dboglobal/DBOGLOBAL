CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1325;
	title = 132502;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 132508;
			gtype = 2;
			oklnk = 2;
			area = 132501;
			goal = 132504;
			sort = 132505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 132502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 132514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 132501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372211;";
			}
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
			stdiag = 132507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "4372211;";
			}
			CDboTSCheckClrQst
			{
				and = "1319;";
				flink = 1;
				not = 0;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 132509;
				ctype = 1;
				idx = 4372211;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 132508;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 132505;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2180.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 132501;
				goal = 132504;
				m0fz = "4491.000000";
				m0widx = 1;
				m1ttip = 132516;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "4494.000000";
				m3ttip = 132516;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "4491.000000";
				m2fx = "2180.000000";
				m2ttip = 132515;
				m0fx = "1828.000000";
				m0ttip = 132515;
				m3pat = 2;
				title = 132502;
				m1fx = "1828.000000";
				m2fz = "4494.000000";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 7111203;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112203;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

