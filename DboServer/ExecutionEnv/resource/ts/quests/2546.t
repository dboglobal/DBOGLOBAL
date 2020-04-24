CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2546;
	title = 254602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2546;
				start = 0;
				taid = 1;
				uof = 0;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 254609;
				ctype = 1;
				idx = 1121101;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2546;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 254608;
				gtype = 3;
				area = 254601;
				goal = 254604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 254605;
				stype = 2;
				taid = 1;
				title = 254602;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 568;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 254614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 254601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1121101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 254607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1121101;";
			}
			CDboTSCheckClrQst
			{
				and = "2545;";
				flink = 1;
				flinknextqid = "2547;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 254608;
			gtype = 3;
			oklnk = 2;
			area = 254601;
			goal = 254604;
			sort = 254605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 254602;
		}
	}
}

