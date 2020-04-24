CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 971;
	title = 97102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 97109;
				ctype = 1;
				idx = 3143102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 97108;
				gtype = 1;
				area = 97101;
				goal = 97104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 97105;
				stype = 2;
				taid = 1;
				title = 97102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 97107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 35;
			}
			CDboTSClickNPC
			{
				npcidx = "3143102;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "976;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 97108;
			gtype = 1;
			oklnk = 2;
			area = 97101;
			goal = 97104;
			sort = 97105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 97102;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 97114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 97101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511207;";
			}
		}
	}
}

