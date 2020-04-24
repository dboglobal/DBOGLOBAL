CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3731;
	title = 373102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 373101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3204301;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 373109;
				ctype = 1;
				idx = 3204101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 373108;
				gtype = 1;
				area = 373101;
				goal = 373104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 373105;
				stype = 2;
				taid = 1;
				title = 373102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 373108;
			gtype = 1;
			oklnk = 2;
			area = 373101;
			goal = 373104;
			sort = 373105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 373102;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSCheckClrQst
			{
				and = "3730;";
				flink = 0;
				flinknextqid = "3732;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3204101;";
			}
		}
	}
}

