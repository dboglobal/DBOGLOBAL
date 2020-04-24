CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 24;
	rq = 0;
	tid = 3823;
	title = 382302;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 382307;
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
				npcidx = "3191101;";
			}
			CDboTSCheckClrQst
			{
				and = "3755;";
				flink = 0;
				flinknextqid = "3756;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 382314;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 382301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3191101;";
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
				conv = 382309;
				ctype = 1;
				idx = 3191101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 13001100;
				cnt0 = 50;
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
				cont = 382308;
				m1fx = "-4154.060059";
				m1fy = "0.000000";
				sort = 382305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5109.490234";
				grade = -1;
				m0fx = "-3848.659912";
				m0ttip = 382315;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				title = 382302;
				gtype = 2;
				area = 382301;
				goal = 382304;
				m0fz = "-5397.129883";
				m0widx = 15;
				m1ttip = 382315;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 382308;
			gtype = 2;
			oklnk = 2;
			area = 382301;
			goal = 382304;
			sort = 382305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 382302;
		}
	}
}

