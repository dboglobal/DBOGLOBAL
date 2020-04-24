CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6897;
	title = 689702;

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
				itype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 689707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "3242121;";
			}
			CDboTSCheckClrQst
			{
				and = "6896;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 689709;
				ctype = 1;
				idx = 3242121;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7513103;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 689708;
				area = 689701;
				goal = 689704;
				m0fz = "1148.729980";
				m0widx = 1;
				scitem = -1;
				sort = 689705;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "4802.759766";
				m0ttip = 689715;
				rwd = 100;
				taid = 1;
				title = 689702;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 689708;
			gtype = 1;
			oklnk = 2;
			area = 689701;
			goal = 689704;
			sort = 689705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 689702;
		}
		CDboTSContGAct
		{
			cid = 4;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 689714;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 689701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242121;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
	}
}

