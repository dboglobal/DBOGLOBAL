CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3033;
	title = 303302;

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
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 303307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "4531105;";
			}
			CDboTSCheckClrQst
			{
				and = "3032;";
				flink = 0;
				flinknextqid = "3034;";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 303314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 303301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4531105;";
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
			cont = 303308;
			gtype = 2;
			oklnk = 2;
			area = 303301;
			goal = 303304;
			sort = 303305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 303302;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 303309;
				ctype = 1;
				idx = 4531105;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 15931202;
				cnt0 = 10;
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
				cont = 303308;
				m1fx = "3462.989990";
				m1fy = "0.000000";
				sort = 303305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4988.939941";
				grade = 132203;
				m0fx = "3260.870117";
				m0ttip = 303315;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 303302;
				gtype = 2;
				area = 303301;
				goal = 303304;
				m0fz = "-4974.390137";
				m0widx = 1;
				m1ttip = 303315;
				scitem = -1;
				stype = 2;
			}
		}
	}
}

