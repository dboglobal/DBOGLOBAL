CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2903;
	title = 290302;

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
			stdiag = 290307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
			CDboTSCheckClrQst
			{
				and = "2902;";
				flink = 0;
				flinknextqid = "2905;";
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
				conv = 290309;
				ctype = 1;
				idx = 4511312;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 290308;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 290305;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2257.830078";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 290301;
				goal = 290304;
				m0fz = "-2611.300049";
				m0widx = 1;
				m1ttip = 290315;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-2481.689941";
				m3ttip = 290316;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2520.020020";
				m2fx = "2583.879883";
				m2ttip = 290316;
				m0fx = "2203.330078";
				m0ttip = 290315;
				m3pat = 2;
				title = 290302;
				m1fx = "2644.449951";
				m2fz = "-2517.989990";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 10;
				eitype = 0;
				idx0 = 17113200;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 17112100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 290308;
			gtype = 2;
			oklnk = 2;
			area = 290301;
			goal = 290304;
			sort = 290305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 290302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 290314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 290301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

