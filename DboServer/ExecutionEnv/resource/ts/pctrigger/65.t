CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 27;
	rq = 0;
	tid = 65;
	title = 6502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 6307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSColObject
			{
				objidx = "1;";
				widx = 900040;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 253;
			prelnk = "0;";

			CDboTSActPIDgn
			{
				hidx = -1;
				ridx = -1;
				nidx = -1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;";
			type = 0;
		}
	}
}

