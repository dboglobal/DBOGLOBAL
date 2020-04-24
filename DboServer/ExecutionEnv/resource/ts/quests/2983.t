CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2983;
	title = 298302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			stdiag = 298307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4751315;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSCheckClrQst
			{
				and = "2982;";
				flink = 0;
				flinknextqid = "2984;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 298314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 298301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3132105;";
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
			cont = 298308;
			gtype = 1;
			oklnk = 2;
			area = 298301;
			goal = 298304;
			sort = 298305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 298302;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 298309;
				ctype = 1;
				idx = 4751315;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 298308;
				area = 298301;
				goal = 298304;
				m0fz = "-4158.680176";
				m0widx = 1;
				scitem = -1;
				sort = 298305;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4945.040039";
				m0ttip = 298315;
				rwd = 100;
				taid = 1;
				title = 298302;
				gtype = 1;
			}
		}
	}
}

