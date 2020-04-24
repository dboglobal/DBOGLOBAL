CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 909;
	title = 90902;

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
				conv = 90909;
				ctype = 1;
				idx = 5061109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 90908;
				gtype = 1;
				area = 90901;
				goal = 90904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 90905;
				stype = 2;
				taid = 1;
				title = 90902;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			desc = 90914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 90901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 90908;
			gtype = 1;
			oklnk = 2;
			area = 90901;
			goal = 90904;
			sort = 90905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 90902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 90907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSCheckClrQst
			{
				and = "908;";
				flink = 1;
				flinknextqid = "910;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5061109;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

