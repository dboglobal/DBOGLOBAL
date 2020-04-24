CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3773;
	title = 377302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 377309;
				ctype = 1;
				idx = 3333101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 50;
				eitype = 0;
				idx0 = 18181100;
				cnt0 = 25;
				cnt2 = 25;
				ectype = -1;
				etype = 0;
				idx1 = 17551100;
				taid = 3;
				esctype = 0;
				idx2 = 13252103;
			}
			CDboTSActRegQInfo
			{
				cont = 377308;
				m1fx = "-4552.790039";
				m2fz = "-5815.680176";
				m1fy = "0.000000";
				sort = 377305;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 377301;
				goal = 377304;
				m0fz = "-5965.180176";
				m0widx = 15;
				m1ttip = 377316;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-79.690002";
				m0pat = 2;
				m1fz = "-6026.779785";
				m2fx = "-4781.169922";
				m2ttip = 377317;
				m0fx = "-4336.049805";
				m0ttip = 377315;
				title = 377302;
			}
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
				taid = 255;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377307;
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
				and = "3771;";
				flink = 0;
				flinknextqid = "3774;";
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377308;
			gtype = 2;
			oklnk = 2;
			area = 377301;
			goal = 377304;
			sort = 377305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 377302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 377314;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 377301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333201;";
			}
		}
	}
}

