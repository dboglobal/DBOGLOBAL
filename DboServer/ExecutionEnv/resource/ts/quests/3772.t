CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3772;
	title = 377202;

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377207;
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
				npcidx = "5066101;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3833;";
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
				conv = 377209;
				ctype = 1;
				idx = 5066101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 30;
				eitype = 0;
				idx0 = 15992103;
				cnt0 = 30;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 17843102;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 377208;
				m3widx = 15;
				m1fy = "0.000000";
				sort = 377205;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-4147.240234";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 377201;
				goal = 377204;
				m0fz = "-7042.259766";
				m0widx = 15;
				m1ttip = 377215;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-6853.689941";
				m3ttip = 377216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-6870.189941";
				m2fx = "-4073.709961";
				m2ttip = 377216;
				m0fx = "-4241.120117";
				m0ttip = 377215;
				m3pat = 2;
				title = 377202;
				m1fx = "-3623.479980";
				m2fz = "-6932.600098";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 377214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 377201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5066101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377208;
			gtype = 2;
			oklnk = 2;
			area = 377201;
			goal = 377204;
			sort = 377205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 377202;
		}
	}
}

