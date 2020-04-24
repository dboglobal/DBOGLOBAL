CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2962;
	title = 296202;

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
			stdiag = 296207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4511215;";
			}
			CDboTSCheckClrQst
			{
				and = "2961;";
				flink = 0;
				flinknextqid = "2970;";
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
				conv = 296209;
				ctype = 1;
				idx = 4511215;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 296208;
				m1fx = "4590.810059";
				m1fy = "0.000000";
				sort = 296205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4387.339844";
				grade = 132203;
				m0fx = "4131.790039";
				m0ttip = 296215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 296202;
				gtype = 2;
				area = 296201;
				goal = 296204;
				m0fz = "-4537.500000";
				m0widx = 1;
				m1ttip = 296215;
				scitem = -1;
				stype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 11531100;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 296201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511215;";
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
			cont = 296208;
			gtype = 2;
			oklnk = 2;
			area = 296201;
			goal = 296204;
			sort = 296205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296202;
		}
	}
}

